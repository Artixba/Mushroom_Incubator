from django.db import models
from django.urls import reverse
from django.contrib import auth


# Create your models here.
class UserProfileInfo(auth.models.User, auth.models.PermissionsMixin):

    Email = models.EmailField(max_length=254,default='')
    Username = models.CharField(max_length=256,default='')


    def get_absolute_url(self):
        return reverse("index",kwargs={'pk':self.pk})

    def __str__(self):
        return "@{}".format(self.username)

class Contact(models.Model):
    first_name = models.CharField(max_length=50,default='')
    last_name = models.CharField(max_length=50,default='')
    email = models.EmailField()
    message = models.TextField()

    def __str__(self):
        return f'{self.first_name} {self.last_name}'

    def save(self, *args, **kwargs):
        super().save(*args, **kwargs)

    def get_absolute_url(self):
        return reverse("index")


   