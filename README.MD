# 🎮 Quake-like 3D Engine

Um **motor 3D inspirado em Quake**, implementado em **C++** com **SDL3** e **ImGui**, permitindo manipulação de câmeras, iluminação, texturas e modos de renderização.

## TL;DR

### To run it.

On Linux:

```bash
xmake && xmake run
```

On Windows:

```bash
xmake; xmake run
```

---

## 🛠 Tecnologias

- **C++20**
- **SDL3** (janela, input e renderização)
- **ImGui + ImGui-SDL3** (UI para debug e controles)
- **Pipeline próprio** para renderização 3D (Flat, Gouraud, Phong, Textured, Wireframe)
- **Z-buffer** para profundidade
- **Controle de câmera Arcball** (com mouse ou via UI)

---

## 🎯 Funcionalidades

- **Movimentação do jogador**:
  - Frente, trás, lateral, subida e descida
  - Rotação com yaw e pitch
- **Câmera Arcball**:
  - Controle com mouse ou checkboxes na interface
  - Rotação livre em X e Y
- **Renderização**:
  - Modos de iluminação: `FLAT`, `GOURAUD`, `PHONG`, `TEXTURED`
  - Wireframe toggle
  - Texturas BMP aplicadas em malhas
- **UI de debug**:
  - Informações do player: posição e target (lookAt)
  - Configurações da cena: iluminação e wireframe
  - Arcball control via checkboxes

---

## 🖼 Layout da UI

- **Viewport principal** à direita
- **Player Info** à esquerda, topo
- **Scene Settings** abaixo do Player Info
- **Arcball Control** integrado à Scene Settings

---

## ⌨️ Controles

- **W/A/S/D**: mover player
- **SPACE / LCTRL**: subir/descer
- **SETAS**: rotacionar yaw/pitch
- **Checkboxes Arcball**: rotação contínua da câmera
