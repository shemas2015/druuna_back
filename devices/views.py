from rest_framework import viewsets,mixins
from rest_framework.permissions import IsAuthenticated
from devices.models import Device
from devices.serializers import DeviceModelSerializer



# Create your views here.



class DeviceViewSet(viewsets.ModelViewSet):
    queryset = Device.objects.all()
    serializer_class = DeviceModelSerializer


    