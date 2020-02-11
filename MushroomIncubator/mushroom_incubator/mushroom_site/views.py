from django.shortcuts import render, get_object_or_404, redirect
from django.urls import reverse_lazy
from django.contrib.auth.decorators import login_required
from django.utils import timezone
from django.contrib.auth.mixins import LoginRequiredMixin
from django.views.generic import (TemplateView,ListView,DetailView,CreateView,UpdateView,DeleteView)



# Create your views here.
class indexView(TemplateView):
    template_name = 'index.html'
    
    
class aboutView(TemplateView):
    template_name = 'about.html'

class contactView(TemplateView):
    template_name = 'contact.html'

class placeholderView(TemplateView):
    template_name = 'placeholder.html'

