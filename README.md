# cnc_plotter
<p align="justify">
Computer Numerical Controlled (CNC) plotters have diverse applications, ranging from computer-aided and architectural drawing to PCB printing. They can even be adapted for printing signatures and stamps for <b>person with disabilities</b>. While CNC plotters are versatile, <b>cost-effectiveness</b> remains a key priority.

Most existing systems rely on open-source or custom-built software for G-code generation, streaming, and parsing. Although effective, these approaches depend heavily on a personal computer and can become <b>cumbersome and time-consuming</b>, especially when frequent modifications to input data are required.

This CNC plotter implementation, based on the recommendation by <b>Joshua Ayanlade et al</b>, addresses this limitation. It <b>eliminate</b> external G-code streaming software such as Universal G-code Sender and Processing, by using the <em>Send-Response</em> protocol to interface with <b>Grbl</b>. Also, it can generates G-code for geometries like <b>vertical, horizontal, and inclined lines, circles, arcs, and bounding boxes</b> directly from user-provided parameters. This approah enables direct <b>human-machine interaction</b> and streamlines the entire plotting process.
</p>

---

### ✨ Features
✅ Eliminates the need for PC-based G-code streaming software.  
✅ Generate g-code for basic geometries (circle, vertical/horizontal line, arc, bounding box).   
✅ Three control modes: Manual, Semi-Automatic, and Automatic.  
✅ Human-machine interface using LCD, push buttons, IR remote, and joystick.  
✅ Cost-effective, scalable, and suitable for diverse applications.  

---

### 🛠️ Hardware Components
- Atmega 2560 microcontroller
- 2004 LCD display
- Rotary encoder
- Push buttons
- Joystick module
- IR sensor and remote
- Stepper motors with drivers
- SD card module
- V3 Arduino CNC shield

---

### 📚 Libraries
- [`LiquidCrystal.h`](https://github.com/Joshua-Ayanlade/cnc_plotter/tree/main/libraries/Arduino-LiquidCrystal-I2C-library-master)
- [`IRremote.h`](https://github.com/Joshua-Ayanlade/cnc_plotter/tree/main/libraries/IRremote)
- [`Encoder.h`](https://github.com/Joshua-Ayanlade/cnc_plotter/tree/main/libraries/Encoder-master)
- [`grbl.h`](https://github.com/Joshua-Ayanlade/cnc_plotter/tree/main/libraries/grbl-mi)

---

### ⚡ Control Modes
- Automatic Mode → Select and execute predefined geometries.
- Semi-Automatic Mode → Input geometry parameters (radius, coordinates, feedrate) and auto-generate G-code.  
- Manual Mode → Control the plotter directly using a joystick for positioning.

---

### 🔌 Circuit Diagram
<p align="center">
<img width="720" height="650" alt="Schematic_cnc plotter_2023-03-13" src="https://github.com/user-attachments/assets/904301c6-060d-4daf-839d-c7dad986c6ed" />
</p>

---

### 📐 CAD Drawing
<p align="center">
<img width="710" height="399" alt="assembly drawing 1" src="https://github.com/user-attachments/assets/0dde119d-25c6-493f-b96a-edb0332375ed" />
</p>

---

### 🖥️ Prototype Demo





<p align="center">
  <table>
    <tr>
      <td align="center" width="50%">
        <img src="https://github.com/user-attachments/assets/266a09ac-c34a-46cd-b28d-405fc7146157" alt="Video 1" style="width: 100%;" />
        <br>
        <strong>Automatic_Mode Control</strong>
      </td>
      <td align="center" width="50%">
        <img src="https://github.com/user-attachments/assets/de235d57-b5e8-491c-b55a-a6c2f0867a65" alt="Video 2" style="width: 100%;" />
        <br>
        <strong>Semi-Automatic Control</strong>
       <p><a href="https://drive.google.com/file/d/12DyQr7QH69gvXZbXd1bE_taHWjEv5YDD/view">Clearer Video</a></p>
      </td>
    </tr>
  </table>
</p>

---

### 🚀 Future Improvements
🌐 IoT integration for remote operation.
---

### 🌟 Acknowledgement

**Grbl** library is developed by **Sungeun K. Jeon**

---
### 📖 References
J. Ayanlade, V. Ayanwunmi, P. Omoniyi and T. C. Jen, "Exploring Creative applications in the Digital Age: A review of Computer Numerical Controlled Plotter," 2023 2nd International Conference on Multidisciplinary Engineering and Applied Science (ICMEAS), Abuja, Nigeria, 2023, pp. 1-5, doi: 10.1109/ICMEAS58693.2023.10379222.

D. Naokes, “Interfacing with Grbl · grbl/grbl Wiki · GitHub.” Accessed: September 10, 2025. [Online]. https://github.com/grbl/grbl/wiki/interfacing-with-grbl
