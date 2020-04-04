from django.views.generic import TemplateView, CreateView
from . import forms, models
from .forms import ContactForm
from django.urls import reverse_lazy
from django.http import HttpResponseRedirect
from django.shortcuts import render
# Pages
class indexView(TemplateView):
    template_name = 'index.html'


class aboutView(TemplateView):
    template_name = 'about.html'

class contactView(CreateView):
    template_name = 'contact.html'
    redirect_field_name = 'index.html'
    form_class = forms.ContactForm

    

class placeholderView(TemplateView):
    template_name = 'placeholder.html'

class howView(TemplateView):
    template_name = 'how.html'
