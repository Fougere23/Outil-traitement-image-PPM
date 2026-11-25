#!/bin/bash
# Utilisation : ./traiter.sh image.pgm filtre
# Exemple     : ./traiter.sh images/test.pgm negatif

# 1) Vérifier les arguments
if [ $# -ne 2 ]; then
    echo "Usage : 0image.pgm|image.ppmfiltre"echo"Filtres:negatifgrisfloumedianascii"exit1fiENTREE="1"
FILTRE="2"if[!-f"ENTREE" ]; then
    echo "Erreur : le fichier ENTREEn'existepas"exit1fi#2)Vérifierquel'outildeconversionPNGestinstalléif!command-vpnmtopng>/dev/null;thenecho"Erreur:installeznetpbmavec:sudoaptinstallnetpbm"exit1fi#3)CompilerleprogrammeCmake-s||echo"Erreurdecompilation";exit1;#4)Appliquerlefiltre(sortiedanslemêmeformatquel'entrée)EXT="{ENTREE##*.}"
SORTIE="web/sortie.EXT"./convert"ENTREE" "SORTIE""FILTRE" || { echo "Erreur : le filtre a échoué"; exit 1; }

# 5) Convertir avant/après en PNG pour le navigateur
pnmtopng "ENTREE">web/avant.pngpnmtopng"SORTIE" > web/apres.png
rm -f "$SORTIE"

# 6) Générer la page HTML
cat > web/index.html <<HTML
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<title>TERANGA - Avant / Après</title>
<style>
  body { font-family: sans-serif; text-align: center; background: #f4f4f4; }
  .images { display: flex; justify-content: center; gap: 30px; flex-wrap: wrap; }
  figure { margin: 0; }
  img { max-width: 400px; width: 100%; image-rendering: pixelated; border: 1px solid #999; }
  .comparaison { position: relative; display: inline-block; margin-top: 30px; }
  .comparaison img { display: block; }
  #calque { position: absolute; top: 0; left: 0; height: 100%; overflow: hidden; width: 50%; }
  #calque img { max-width: none; }
  input[type=range] { width: 400px; max-width: 100%; }
</style>
</head>
<body>
  <h1>Filtre : $FILTRE</h1>
  <p>Image : (basename"ENTREE")</p>

  <div class="images">
    <figure><img src="avant.png" alt="Avant"><figcaption>Avant</figcaption></figure>
    <figure><img src="apres.png" alt="Après"><figcaption>Après</figcaption></figure>
  </div>

  <h2>Comparaison avec curseur</h2>
  <div class="comparaison" id="boite">
    <img src="apres.png" id="fond" alt="Après">
    <div id="calque"><img src="avant.png" id="dessus" alt="Avant"></div>
  </div>
  <br>
  <input type="range" id="curseur" min="0" max="100" value="50">

  <script>
    const curseur = document.getElementById("curseur");
    const calque  = document.getElementById("calque");
    const fond    = document.getElementById("fond");
    const dessus  = document.getElementById("dessus");
    function maj() {
      dessus.style.width = fond.clientWidth + "px";   /* même taille que l'image du fond */
      calque.style.width = curseur.value + "%";        /* on montre une partie de "avant" */
    }
    curseur.addEventListener("input", maj);
    window.addEventListener("resize", maj);
    fond.addEventListener("load", maj);
    maj();
  </script>
</body>
</html>
HTML

echo "Terminé ! Ouvrez web/index.html dans le navigateur."