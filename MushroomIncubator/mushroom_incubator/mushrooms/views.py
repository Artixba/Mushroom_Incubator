from django.shortcuts import render, get_object_or_404
from django.http import Http404,HttpResponse, HttpRequest
from urllib.request import urlopen
import json, requests
from django.urls import reverse_lazy
from django.contrib.auth.mixins import LoginRequiredMixin
from django.views.generic import TemplateView, CreateView, DeleteView, UpdateView, DetailView, ListView
from . import models
from .forms import MushroomForm
from .models import Mushroom

from django.contrib.auth import get_user_model
User = get_user_model()
# Create your views here.

class SampleChoiceView(TemplateView):
    template_name = 'sample_choice.html'
    form_class = MushroomForm

def exampleView(request):
    auth_token = "jWL3x5xRTSLnDnXZyuXxtNiznJ8ikLAm"
    pin = "V6"
    
    apirequest = (f'http://blynk-cloud.com/{auth_token}/get/{pin}')
    r = requests.get(apirequest)
    context = {
        'hello': 'hellos',
        'num1': 1254,
        'num2': 536,
        'response': r
    }
    
    return render(request,'mushrooms/sample_choice.html', context)

class CreateMushroomView(LoginRequiredMixin, CreateView):
    fields = ('title', 'mushroom_choices')
    model = models.Mushroom

class UpdateMushroomView(UpdateView):
    fields = ('title')
    model = models.Mushroom

class DeleteMushroomView(LoginRequiredMixin, DeleteView):
    model = models.Mushroom
    success_url = reverse_lazy('accounts:profile')

class MushroomDetailView(DetailView):
    model = models.Mushroom
    template_name = 'mushrooms/mushroom_detail.html'

class MushroomListView(ListView):
    model = models.Mushroom
    template_name = 'mushrooms/mushroom_list.html'
    context_object_name = 'mushrooms'

