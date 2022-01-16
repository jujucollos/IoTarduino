# PlantaX

PlantaX est un appareil permettant de mesurer 3 facteurs importants dans la pousse d'une plante : 
- Le niveau d'humidité de l'air
- Le niveau d'humidité dans la terre
- Le niveau d'eau

Ces données seront transmises à une base de données en ligne (InfluxDB) ce qui vous permettra de suivre l'évolution des conditions de pousse de votre plante.


## Comment utiliser PlantaX

Créez un compte sur InfluxDB Cloud (https://www.influxdata.com/products/influxdb-cloud/) puis créez un bucket. Une fois ceci fait, vous pouvez générer un token qui permettra à PlantaX d'écrire sur la base de données. Enfin, dans le fichier de configuration de PlantaX remplacez les éléments d'authentification par défauts par vos informations. 

Ensuite, plantez les capteurs d'humidité dans la terre et du niveau d'eau dans le pot de votre plante et laissez les deux autres capteurs sur le dessus du pot et les données seront automatiquement transmises vers la base de données.

Sur InfluxDB, vous pouvez cliquer sur "RUN" dans votre bucket pour voir immédiatement les données sous forme de graphique.

## Membres
- Guillaume Joyal
- Adrien Devouassoux
- Maxime Agnola
- Julien Collos
