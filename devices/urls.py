from django.urls import include, path
from rest_framework.routers import DefaultRouter
from . import views as devices_views

router = DefaultRouter()
router.register(r'devices',  devices_views.DeviceViewSet , basename='devices')
urlpatterns = [
    path('', include(router.urls))
]
