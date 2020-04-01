# Generated by Django 2.2.5 on 2020-03-29 22:12

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('mushroom_site', '0002_auto_20200225_1552'),
    ]

    operations = [
        migrations.CreateModel(
            name='Contact',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('first_name', models.CharField(max_length=200)),
                ('last_name', models.CharField(max_length=200)),
                ('email', models.EmailField(max_length=254)),
                ('message', models.TextField()),
            ],
        ),
        migrations.AlterField(
            model_name='userprofileinfo',
            name='Email',
            field=models.EmailField(default='', max_length=254),
        ),
        migrations.AlterField(
            model_name='userprofileinfo',
            name='Username',
            field=models.CharField(default='', max_length=256),
        ),
    ]
