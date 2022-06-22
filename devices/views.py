from email.quoprimime import unquote
from xmlrpc.client import ResponseError
from django.http.response import FileResponse,HttpResponse
from requests import delete
from rest_framework.response import Response
from rest_framework import viewsets,status
from rest_framework.permissions import IsAuthenticated
from devices.models import Device, Page, PageConfig, UserDevice
from devices.serializers import ConfigModelSerializer, DeviceModelSerializer, DeviceUserModelSerializer, PageModelSerializer
from rest_framework.decorators import action



# Create your views here.



class DeviceViewSet(viewsets.ModelViewSet):
    queryset            = Device.objects.all()
    serializer_class    = DeviceModelSerializer
    permission_classes  = [IsAuthenticated]

    @action(detail=False, methods=['GET'], name='Return device img' , url_path=r'devices_img/(?P<file_name>\w+\..*)', )
    def devices_img( self , request , file_name ):
        """
        Download a device image
        """
        image = open('devices/devices_img/{}'.format(file_name), 'rb')

        return HttpResponse(image, content_type="image/jpeg")
    


class DeviceUserViewSet(viewsets.ModelViewSet):
    queryset = UserDevice.objects.all()
    serializer_class = DeviceUserModelSerializer
    permission_classes  = [IsAuthenticated]

    
    @action(detail=False, methods=['POST'], name='Return device img' , url_path=r'get_by_mac', )
    def get_by_mac( self , request ):
        """
        Download a device image
        """

        mac           = request.data["mac"]
        qs            = super().get_queryset()
        serializer    = self.get_serializer(qs.filter(mac=mac), many=True)
        return Response(serializer.data)

    def get_queryset( self ):
        return UserDevice.objects.filter( owner_id = self.request.user.id )

    @action(detail=False, methods=['POST'], name='link a device with a new user' , url_path=r'link_user', )
    def link_user( self , request ):
        """link a device with a new user
        """

        #Find device by mac, if device doesn't exists return error
        device = UserDevice.objects.filter(mac = request.data["mac"] )

        if( device.count() == 0 ):
            return Response({"error":"Device not found"} , status = status.HTTP_400_BAD_REQUEST )
        
        
    
        #If device exists link it with log user
        device            = device.first()
        device.owner_id   = request.user.id
        device.save()

        return Response( "Device was linked successful" )


    


class PageViewSet(viewsets.ModelViewSet):
    queryset            = Page.objects.all()
    serializer_class    = PageModelSerializer
    permission_classes  = [IsAuthenticated]

    

class ConfigViewSet(viewsets.ModelViewSet):
    queryset            = PageConfig.objects.all()
    serializer_class    = ConfigModelSerializer
    permission_classes  = [IsAuthenticated]
    

    #Consulta únicamente la configuración del usuario logueado (get all)
    def list(self ,request):
        configs = PageConfig.objects.filter(user_device__owner = self.request.user.id)
        serialize = ConfigModelSerializer(configs , many=True )
        return Response(serialize.data)
        

    def create(self,request):
        data    = request.data
        owner   = UserDevice.objects.get(id = data["user_device"])
        if owner.owner.id != self.request.user.id:
            return Response({"error" : "This device does not belong to the user"},status = status.HTTP_400_BAD_REQUEST)

        return super().create(request)

    #Editar - Si el dispositivo no pertenece al usuario logueado no permite hacer la conexión
    def update(self , request , pk ):
        config = PageConfig.objects.get(id = pk)
        if config.user_device.owner_id !=  self.request.user.id:
            return Response({"error" : "This device does not belong to the user"},status = status.HTTP_400_BAD_REQUEST)

        request.data._mutable = True
        request.data.update({"user_device": config.user_device.id})
        return super().update(  request )

    def destroy( self,request,pk ):
        config = PageConfig.objects.get(id = pk)
        if config.user_device.owner_id !=  self.request.user.id:
            return Response({"error" : "This device does not belong to the user"},status = status.HTTP_400_BAD_REQUEST)

        return super().destroy( request )


    



    #Get By id - Si el dispositivo no pertenece al usuario logueado no permite hacer la conexión

    