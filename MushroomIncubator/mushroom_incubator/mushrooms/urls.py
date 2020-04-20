from django.contrib import admin
from django.urls import path, include
from . import views

app_name = 'mushrooms'

urlpatterns = [
    path('sample/',views.SampleChoiceView.as_view(template_name='mushrooms/sample_choice.html'),name='sample'),
]