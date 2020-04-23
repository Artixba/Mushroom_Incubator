from django.contrib import admin
from mushroom_site.models import UserProfileInfo, Contact
from mushrooms.models import Mushroom

# Register your models here.

class MushroomAdmin(admin.ModelAdmin):
    fields = ['last_name','first_name','email', 'message']
    search_fields=["first_name","last_name"]
    list_display = ['last_name','first_name','email', 'message']

class MushroomIncubate(admin.ModelAdmin):
    fields = ['user', 'created_at', 'title', 'mushroom_choices']
    search_fields = ['user', 'created_at', 'mushroom_choices']
    list_display = ['user', 'title', 'mushroom_choices','created_at']


admin.site.register(UserProfileInfo)
admin.site.register(Contact,MushroomAdmin)
admin.site.register(Mushroom,MushroomIncubate)