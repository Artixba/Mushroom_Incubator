from django.db import models
from django.urls import reverse
from django.conf import settings
from django.utils import timezone

from mushroom_site.models import UserProfileInfo

from django.contrib.auth import get_user_model 
User = get_user_model() 
# Create your models here.

class Mushroom(models.Model):
    
    user = models.ForeignKey(User,related_name='mushrooms', null=True, blank=True, on_delete=models.CASCADE)
    created_at = models.DateTimeField(default=timezone.now)
    title = models.CharField(max_length=24)
    
    mushroom_type = (
        ('Chanterelle','Chanterelle'),
        ('Shiitake', 'Shiitake'),
        ('Portobello', 'Portobello'),
    )

    mushroom_choices = models.CharField(max_length=30, null=True, choices=mushroom_type)

    def __str__(self):
        return f'{self.user}'

    def get_absolute_url(self):
        return reverse("mushrooms:detail", kwargs={'pk':self.pk})

    class Meta:
        ordering = ['-created_at']
