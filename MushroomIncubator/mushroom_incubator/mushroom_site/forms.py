from django import forms
from django.contrib.auth.models import User
from django.contrib.auth import login, authenticate
from mushroom_site.models import UserProfileInfo
from django.contrib.auth.forms import UserCreationForm
from django.views.generic.edit import FormView
from django.shortcuts import render, redirect
from django.contrib.auth import get_user_model
from django.http import HttpResponseRedirect, request


class RegisterForm(UserCreationForm,forms.ModelForm):

    class Meta():
        model = get_user_model()
        fields = ('username', 'email', 'password1','password2')


    def __init__(self,*args,**kwargs):
        super().__init__(*args, **kwargs)
        self.fields['username'].label = 'Display name'
        self.fields['email'].label = 'Email address'







class UserForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta():
        model = User
        fields = ('username', 'email', 'password')

class UserProfileInfoForm(forms.ModelForm):
    class Meta():
        model = UserProfileInfo
        fields = ('selected_mushroom',)