% Define your Coordinate array
clear

gateways = [
    -34.881516, 150.602400;
    -34.887575, 150.642324;
    -34.911052, 150.719354;
    -34.839796, 150.557023;
    -34.832912, 150.727740;
    -34.809309, 150.627706;
    -35.074462, 150.666042;
    -35.059263, 150.580384;
    -35.152371, 150.571399;
    -35.244884, 150.522281;
    -35.228738, 150.436024;
    -35.329472, 150.450999;
    -35.347552, 150.470167;
];


% Extract latitude and longitude from the Coordinate array
lat = gateways(:, 1);
lon = gateways(:, 2);

% Create a geoplot object
geoplot(lat, lon, 'ko', 'MarkerFaceColor', 'r', 'MarkerSize', 5);
geobasemap satellite
geolimits([-35.8 -34.6],[149.8 150.9])
hold on


initialLat = -35.5;
initialLon = 150.7;
%pointHandle = geoplot(initialLat, initialLon,'ko', 'MarkerFaceColor', 'b', 'MarkerSize', 10);


% Create a movable point using drawpoint
initialLat = -35.7;
initialLon = 150.8;
h = drawpoint('Position', [-35.152371, 150.571399])
% Define a callback function to get the current position
addlistener(h, 'MovingROI', @(src, event) getCurrentPosition(event));


function getCurrentPosition(event)
    newPosition = event.CurrentPosition;
    currentLat = newPosition(2);
    currentLon = newPosition(1);
    
    % Display the current latitude and longitude
    fprintf('Current Latitude: %.6f\n', currentLat);
    fprintf('Current Longitude: %.6f\n', currentLon);
    calllib('geofenceCharlie','geofenceCharlie',currentLat,currentLon);


end

