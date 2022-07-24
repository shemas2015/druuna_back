from posixpath import basename
from django.urls import include, path
from rest_framework.routers import DefaultRouter
from . import views as devices_views

router = DefaultRouter()
router.register(r'devices',  devices_views.DeviceViewSet , basename='devices')
urlpatterns = [
    path('', include(router.urls))
]


router.register(r'devices_user',  devices_views.DeviceUserViewSet , basename='devices_user')
urlpatterns = [
    path('', include(router.urls))
]


router.register(r'pages',  devices_views.PageViewSet , basename='pages')
urlpatterns = [
    path('', include(router.urls))
]


router.register(r'config',  devices_views.ConfigViewSet , basename='pages')
urlpatterns = [
    path('', include(router.urls))
]