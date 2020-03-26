from django.db import models
from django.urls import reverse
from django.conf import settings
from django.contrib.auth import get_user_model
User = get_user_model()

# Create your models here.

class Mushroom(models.Model):

    user = models.ForeignKey(User,related_name='mushrooms', on_delete=models.CASCADE)
    started_on = models.DateTimeField(auto_now=True)
    
    