# Global Maritime Incident App

<p>C++ and Qt application which displays a 3D globe Earth and maritime incidents displayed by spheres on the surface of the Earth.</p>
<p>The globe, country borders, country geometries, and incidents are rendered in OpenGL as a part of Qt's OpenGL-specific widgets.</p>

### Features

<ul>
    <li>Ability to rotate the camera around the Earth.</li>
    <li>Select an incident and display its relevant information.</li>
    <li>Select countries whose ships were reported on/countries of origin where the orchestrators of the attack were.</li>
</ul>

### Technologies and APIs

<ul>
    <li>C++ and Qt (UI + widgets)</li>
    <li>OpenGL (visualize the Earth, render borders, and compute lighting calculations)</li>
    <li>GDELT API to retrieve the maritime incidents</li>
</ul>

### Resources:
<ul>
    <li><a href="https://datahub.io/core/geo-countries">World geojson</a></li>
    <li><a href="https://science.nasa.gov/earth/earth-observatory/blue-marble-next-generation/base-map/">NASA Physical Geography Map</a></li>
</ul>

### Sample Images (as of August 23rd 2026)
<p>(Only borders/user-driven camera rotation were implemented at this time)</p>

<img width="1162" height="769" alt="Image" src="https://github.com/user-attachments/assets/d136cc7d-404f-4ce2-b8c5-d0f5e2cb54a0" />

<img width="1162" height="765" alt="Image" src="https://github.com/user-attachments/assets/12b91655-e5f4-44b2-9e42-08888a2abc12" />

<img width="1168" height="771" alt="Image" src="https://github.com/user-attachments/assets/5512aa4e-05b3-4216-b4e6-5105f733c6a5" />