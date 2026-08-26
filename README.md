# Global Maritime Incident Geospatial Intelligence

<p>C++ and Qt application which displays a 3D globe Earth and maritime incidents displayed by spheres on the surface of the Earth.</p>
<p>The globe, country borders, country geometries, and incidents are rendered in OpenGL as a part of Qt's OpenGL-specific widgets.</p>

### Features

<ul>
    <li>Ability to rotate the camera around the Earth.</li>
    <li>Select an incident and display its relevant information.</li>
    <li>Select countries whose ships were had their flag on/countries of origin where the orchestrators of the attack were.</li>
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


## August 25 Log

### Problems with the naive approach combatting z-fighting results in apparent displacement between the coastline and geojson border alignment:

<p>As the camera distance increases, the apparent displacement between the Earth's surface and the artificially offset geographic overlay becomes noticeable as shown below:</p>

<img width="606" height="610" alt="Image" src="https://github.com/user-attachments/assets/b6321212-14ea-4505-b328-a03f721d7b1b" />

### Alternative method to combatting the z-fighting problem:

<p>As shown in the image above, the values ε0, ε1, ε2, and ε3 demonstrate the distances between the borders and coastlines of Oman, Yemen, Somalia, and Iran respectively. This is due to scaling the radius of the borders as the camera distances itself from the Earth model.</p>

<p>Thus comes a method where the borders and Earth are rendered separately to individual textures, with the borders having only <code>borderTexture</code> and the Earth having both <code>earthTexture</code> and <code>earthDepthTexture</code>.</p> 

<p>The borders do not require any depth texture since we will render the border to a texture that are pointing perpendicular or in the direction of the camera. Rendering this way will neglect the borders rendering on the other side of the planet.</p>

<p>To calculate whether the borders are pointing perpendicular or towards the camera we can use the following: <code>dot(cameraDirection, borderNormal) >= threshold</code> where <code>borderNormal = normalize(borderVertex)</code>. The preceding negates all the geometry physically behind the planet model, removing the need to compute a separate border depth texture.</p>

<p>In the shader, there will be a <code>vec4 composite</code> which will be responsible to compute the final color of the scene. Initially, the composite variable will be computed as such: <code>composite = mix(borderColor, earthColor, borderColor.a)</code>. This will always confirm that the borders are being rendered above the Earth, resulting in zero z-fighting.</p>

<p>Additionally, the scene has its own <code>sceneTexture</code> and <code>sceneDepthTexture</code> so we can render the scene (which contains mostly the incident spheres and country polygons) on top of the Earth. This can be achieved by sampling from both the <code>earthDepthTexture</code> and <code>sceneDepthTexture</code> and see which value is smaller (meaning closer to the camera).</p>

<p>The composite can thus be calculated as <code>composite = sceneDepth < earthDepth ? sceneColor : composite;</code>.</p>

## MSAA Multisampling + Alternative Method Results (Aug 26):

<img width="1175" height="773" alt="Image" src="https://github.com/user-attachments/assets/71a5c4d8-472f-45ff-97bc-7b12bb0045af" />
