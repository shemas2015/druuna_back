
from django.shortcuts import render
from rest_framework import permissions
from django.contrib.auth import get_user_model # If used custom user model
from rest_framework import viewsets
from users.serializers import UserSerializer


# Create your views here.


class UserViewSet( viewsets.ModelViewSet ):
    model = get_user_model()
    permission_classes = [
        permissions.AllowAny # Or anon users can't register
    ]
    serializer_class = UserSerializer