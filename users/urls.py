from django.urls import include, path
from rest_framework.routers import DefaultRouter
from . import views as users_views

router = DefaultRouter()
router.register(r'users',  users_views.UserViewSet , basename='users')
urlpatterns = [
    path('', include(router.urls))
]
