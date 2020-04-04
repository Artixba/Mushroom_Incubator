from django.contrib import admin
from mushroom_site.models import UserProfileInfo, Contact

# Register your models here.

class MushroomAdmin(admin.ModelAdmin):
    fields = ['last_name','first_name','email', 'message']
    search_fields=["first_name","last_name"]
    list_display = ['last_name','first_name','email', 'message']

admin.site.register(UserProfileInfo)
admin.site.register(Contact,MushroomAdmin)