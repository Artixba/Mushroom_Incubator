from django.shortcuts import render
from django.views.generic import TemplateView
from .forms import MushroomForm

# Create your views here.

class SampleChoiceView(TemplateView):
    template_name = 'sample_choice.html'
    form_class = MushroomForm
