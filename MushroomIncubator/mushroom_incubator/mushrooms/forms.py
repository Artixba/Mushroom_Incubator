from django import forms
from django.contrib.auth.models import User
from .models import Mushroom

class MushroomForm(forms.ModelForm):

    class Meta():
        model = Mushroom
        fields = ('title','mushroom_choices')

