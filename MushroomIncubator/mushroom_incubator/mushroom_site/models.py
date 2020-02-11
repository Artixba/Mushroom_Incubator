from django.db import models
from django.urls import reverse
from django.contrib.auth.models import User

# Create your models here.
class UserProfileInfo(models.Model):
    user = models.OneToOneField(User,on_delete=models.CASCADE)

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

    def __str__(self):
        return self.user.username
