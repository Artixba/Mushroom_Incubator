# Generated by Django 2.2.5 on 2020-04-23 16:34

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('mushrooms', '0005_auto_20200423_1132'),
    ]

    operations = [
        migrations.AlterField(
            model_name='mushroom',
            name='mushroom_type',
            field=models.CharField(choices=[('Chanterelle', 'Chanterelle'), ('Shiitake', 'Shiitake'), ('Portobello', 'Portobello')], max_length=30, null=True),
        ),
    ]
