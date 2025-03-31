# MSDOS
# MSDOS: Ferramentas e Recursos para Retrocomputação em MS-DOS e PC386 🕹️💾

[![GitHub](https://img.shields.io/github/license/ppsilv/msdos)](LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/ppsilv/msdos)](https://github.com/ppsilv/msdos/stargazers)
[![DOSBox](https://img.shields.io/badge/Compatível_com-DOSBox-important)](https://www.dosbox.com)

**Desenvolvimento para MS-DOS, PC 386 e hardware retro com C, Pascal e Assembly.** Explore recursos para motherboards, placas de vídeo VGA e drives de disquete!

---

## 📌 Descrição
Repositório dedicado à preservação e desenvolvimento de software para:
- **Sistemas MS-DOS** (6.22, 5.0)
- **Hardware retro**: Intel 386, motherboards ISA/VLB, placas VGA/SVGA e drives de 5.25"/3.5"
- **Linguagens**: C (Turbo C), Pascal (Turbo Pascal), Assembly (MASM/TASM)

---

## 🚀 Recursos
### 🖥️ Ferramentas de Desenvolvimento
- Compiladores: TurboC 2.01, Turbo C++ 3.0, Borland Pascal 7.0, masm 6.11
- Bibliotecas para controle direto de hardware (VGA, DMA)
- Exemplos de código para:
  - Modo gráfico 13h (320x200 256 cores)
  - Acesso direto a portas ISA
  - Manipulação de disquetes via BIOS INT 13h

### 🛠️ Documentação de Hardware
- **Motherboards**:
  - Chipsets comuns (Chips & Technologies, OPTi)
  - Configuração de jumpers e CMOS
- **Placas de Vídeo**:
  - Programação VGA (CRTC, DAC)
  - Suporte a SVGA (Tseng ET4000, Trident)
- **Floppy Disk**:
  - Formatação física de discos (MFM)
  - Recuperação de dados de disquetes antigos

---

## 📋 Tabela de Conteúdos
1. [Começando](#-começando)
2. [Hardware Suportado](#-hardware-documentado)
3. [Exemplos de Código](#-exemplos-de-código)
4. [Contribuição](#-contribuição)

---

## 🛠️ Começando
### Pré-requisitos
- Emulador [DOSBox-X](https://dosbox-x.com/) ou hardware real (PC 386/486)
- Disquete de boot com MS-DOS 6.22

### Clone o Repositório
```bash
git clone https://github.com/ppsilv/msdos.git
