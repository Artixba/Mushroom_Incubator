from django.urls import path
from mushroom_site import views

urlpatterns = [
    path('',views.indexView.as_view(),name='index'),
    path('about/',views.aboutView.as_view(),name='about'),
    path('contact/',views.contactView.as_view(),name='contact'),
    path('placeholder/',views.placeholderView.as_view(),name='placeholder'),
]
