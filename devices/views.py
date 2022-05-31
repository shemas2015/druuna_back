from xmlrpc.client import ResponseError
from django.http.response import FileResponse,HttpResponse
from rest_framework.response import Response
from rest_framework import viewsets,status
from rest_framework.permissions import IsAuthenticated
from devices.models import Device, UserDevice
from devices.serializers import DeviceModelSerializer, DeviceUserModelSerializer
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
        #return FileResponse(image)
        #return Response("setting");
    


class DeviceUserViewSet(viewsets.ModelViewSet):
    queryset = UserDevice.objects.all()
    serializer_class = DeviceUserModelSerializer
    permission_classes  = [IsAuthenticated]

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


    


    