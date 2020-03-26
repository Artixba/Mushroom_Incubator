from django.db import models
from django.urls import reverse
from django.contrib import auth


# Create your models here.
class UserProfileInfo(auth.models.User, auth.models.PermissionsMixin):

    Email = models.EmailField(max_length=254,default='')
    Username = models.CharField(max_length=256,default='')

    Pb = 'Portobello Mushroom'
    Hm = 'Honey Mushroom'
    St = 'Shiitake Mushroom'
    Ct = 'Chaneterelle Mushroom'
    Oy = 'Oyster Mushroom'

    mushroom_choices = [
        (Pb, 'Portobello Mushroom'),
        (Hm, 'Honey Mushroom'),
        (St, 'Shiitake Mushroom'),
        (Ct, 'Chanterelle Mushroom'),
        (Oy, 'Oyster Mushroom'),
        ]

    selected_mushroom = models.CharField(choices=mushroom_choices,max_length=100)

    def get_absolute_url(self):
        return reverse("index",kwargs={'pk':self.pk})

    def __str__(self):
        return "@{}".format(self.username)
