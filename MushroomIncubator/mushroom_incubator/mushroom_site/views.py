from django.views.generic import TemplateView
from . import forms
# Pages
class indexView(TemplateView):
    template_name = 'index.html'


class aboutView(TemplateView):
    template_name = 'about.html'

class contactView(TemplateView):
    template_name = 'contact.html'
    form_class = forms.ContactForm

class placeholderView(TemplateView):
    template_name = 'placeholder.html'

class howView(TemplateView):
    template_name = 'how.html'
