from django.views.generic import TemplateView
# Pages
class indexView(TemplateView):
    template_name = 'index.html'


class aboutView(TemplateView):
    template_name = 'about.html'

class contactView(TemplateView):
    template_name = 'contact.html'

class placeholderView(TemplateView):
    template_name = 'placeholder.html'

class howView(TemplateView):
    template_name = 'how.html'
