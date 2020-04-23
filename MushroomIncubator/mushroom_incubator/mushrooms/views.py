from django.shortcuts import render, get_object_or_404
from django.http import Http404
from django.urls import reverse_lazy
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

class CreateMushroomView(CreateView):
    fields = ('title', 'mushroom_choices')
    model = models.Mushroom
    # template_name = 'create_mushroom.html'

class UpdateMushroomView(UpdateView):
    fields = ('title')
    model = models.Mushroom
    

class DeleteMushroomView(DeleteView):
    model = models.Mushroom
    success_url = reverse_lazy('accounts:profile')

class MushroomDetailView(DetailView):
    # context_object_name = 'mushroom_detail'
    model = models.Mushroom
    template_name = 'mushrooms/mushroom_detail.html'

    # def get_queryset(self):
    #     queryset = super().get_queryset()
    #     return queryset.filter(user__username__iexact=self.kwargs.get('username'))

class MushroomListView(ListView):
    model = models.Mushroom
    template_name = 'mushrooms/mushroom_list.html'
    context_object_name = 'mushrooms'

    # def get_queryset(self):
    #     try:
    #         self.mushroom_user = User.objects.prefetch_related('mushrooms').get(username__iexact=self.kwargs.get('username'))
    #     except User.DoesNotExist:
    #         pass
    #     else:
    #         return self.mushroom_user.mushrooms.all()
    