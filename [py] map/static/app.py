from flask import Flask, request, session, render_template, url_for, redirect
from flask_googlemaps import GoogleMaps, Map, icons
from email.utils import parseaddr # used for checking email formet
from sqlalchemy.orm import sessionmaker
import os, math
from datetime import datetime # datetime is used for saving animal register date
from database import *
app = Flask(__name__)
GoogleMaps(app, key='AIzaSyDZXgn7eF9gWak6swm9i8e75z2aZ-zSyjQ')
# google map key
engine = create_engine('sqlite:///HOTSIX.db', echo=True)

# def PointsInCircum(r,n=5):
#     pi = 3
#     return [(round(math.cos(2*pi/n*x)*r, 4), round(math.sin(2*pi/n*x)*r, 4)) for x in range(0,n+1)]



@app.route('/map',methods = ['GET'])
def map():
    #user_x = request.args.get('user_x', default = 1, type = float)
    #user_y = request.args.get('user_y', default = 1, type = float)
    base = [37.568752, 126.978672]
    
    circle = {
        'stroke_color': '#0000FF',
        'stroke_opacity': .5,
        'stroke_weight': 5,
        'fill_color': '#FFFFFF',
        'fill_opacity': .2,
        'center': {
                  'lat': base[0],
                  'lng': base[1]
        },
        'radius': 17,
    }
    
    circlemap = Map(
        identifier="map", varname="map",
        lat=base[0], lng=base[1], zoom = 20,
        markers=[
            {
                'icon': 'http://maps.google.com/mapfiles/ms/icons/yellow-dot.png',
                'lat':  37.568752,
                'lng':  126.978672,
                'infobox': "31 ug/m3"
            },
            {
                'icon': 'http://maps.google.com/mapfiles/ms/icons/blue-dot.png',
                'lat': 37.568782,
                'lng': 126.978682,
                'infobox': "33 ug/m3"
            },
        ],
        
        circles=[circle]
    )
    return render_template('map.html', map=circlemap)

@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html'), 404

if __name__ == '__main__':
    app.secret_key = os.urandom(12)
    app.run(host='0.0.0.0')
