from django.contrib import admin
from django.urls import path, include
from . import views

app_name = 'mushrooms'

urlpatterns = [
    path('sample/',views.SampleChoiceView.as_view(template_name='mushrooms/sample_choice.html'),name='sample'),
    path('create/',views.CreateMushroomView.as_view(), name='create'),
    path('delete/<int:pk>/',views.DeleteMushroomView.as_view(template_name='mushrooms/delete_mushroom.html'), name='delete'),
    path('<int:pk>/',views.MushroomDetailView.as_view(), name='detail'),
    path('',views.MushroomListView.as_view(), name='list'),
]