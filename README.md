## Zetaman

A famous platformer game

Instalación:

```bash
# Dependencias básicas
apt-get install build-essential git
# Dependencias del juego
apt-get install libgtkmm-3.0-dev libbox2d-dev

# Disclaimer, la libreria que usamos dice no funcionar bien con gcc 4.8
# Para actualizar la versión de gcc en ubuntu se puede actualizarlo como dice acá:
# http://askubuntu.com/questions/466651/how-do-i-use-the-latest-gcc-on-ubuntu
# Pero hasta ahora no tuvimos problema usando 4.8

# Para obtener y compilar el código:
git clone https://gitlab.com/gseva/zetaman.git
cd zetaman
make

# En la carpeta build quedan 3 ejecutables
./build/server   # Servidor
./build/zm       # Cliente
./build/editor   # Editor
```
