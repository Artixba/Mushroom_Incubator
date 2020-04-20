from django.db import models
from django.urls import reverse
from django.conf import settings

from django.contrib.auth import get_user_model 
User = get_user_model() 
# Create your models here.

class Mushroom(models.Model):
    
    user = models.ForeignKey(User,related_name='mushrooms', on_delete=models.CASCADE)
    created_at = models.DateTimeField(auto_now=True)
    title = models.CharField(max_length=24)

    mushroom_choices = (
        ('Chanterelle','Chanterelle'),
        ('Shiitake', 'Shiitake'),
        ('Portobello', 'Portobello'),
    )

    mushroom_type = models.CharField(max_length=30, blank=True,null=True, choices=mushroom_choices)

    def __str__(self):
        return self.title

    def save(self,*args,**kwargs):
        self.title_html = self.title
        super().save(*args,**kwargs)

    def get_absolute_url(self):
        return reverse("accounts:profile",kwargs={"username": self.user.username,"pk":self.pk})

    class Meta:
        ordering = ['-created_at']
