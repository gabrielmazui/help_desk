# 🖥️ Help Desk System (C)

![C Language](https://img.shields.io/badge/Language-C-blue?style=flat-square)
![UFSM](https://img.shields.io/badge/Institution-UFSM-orange?style=flat-square)

## 📖 Overview

This project is a **console-based Help Desk system** written in **C**, developed as a coursework project for the **Data Structures course at UFSM**.
It allows multiple types of users to interact with a ticketing system, manage inventory, and track ongoing tasks efficiently.

The system emphasizes **terminal aesthetics**, using **ANSI colors**, **keyboard arrow navigation**, and **dynamic terminal resizing support** for both Windows and Linux. Menus and user interactions are designed to be **intuitive, responsive, and visually organized**.

---

## 👥 User Roles & Permissions

| Role                | Capabilities                                                                                                        |
| ------------------- | ------------------------------------------------------------------------------------------------------------------- |
| **Client**          | Create new tickets, view their own tickets                                                                          |
| **Technician**      | Handle assigned tickets, conclude tickets, view ticket details                                                      |
| **Manager**         | View tickets, move tickets to "In Progress" or "Cancelled", manage inventory, restock items, view suspended tickets |
| **Administrator**   | Same as Manager                                                                                                     |
| **Mazui (Creator)** | Same as Manager/Admin
---

## 🛠️ Main Features

### Ticket Management

* Create new tickets
* Track tickets: **Open → In Progress → Closed / Cancelled**
* Suspend tickets if inventory is insufficient
* Automatic assignment of tickets to available attendants based on workload and ID priority
* Attendants can only **conclude tickets**

### Inventory Management

* Add new inventory items
* Update existing stock quantities
* View pending restock items
* Interactive menu using arrow keys to adjust quantities
* Dynamic stock checking for pending tickets

### User Management

* Admins and Mazui can manage users (add/edit/delete)
* View user-specific tickets
* Filter users by role (creator/admin/client/technician)

### Terminal & UI

* Cross-platform support for **Windows** and **Linux**
* Arrow-key navigation with `Enter` selection
* ANSI color codes for highlighting menus, tickets, and messages
* Dynamic terminal resizing detection
* Optimized screen updates using line-by-line refresh (no full screen clearing)

---

## 🗂️ Project Structure

```
├── include/  
│   └── *.h                     # Header files for functions
├── src/  
│   ├── app/  
│   │   ├── chamados/           # Ticket logic (CRUD)
│   │   ├── estoque/            # Inventory logic
│   │   └── menus/              # Menu navigation and user interactions
│   ├── extras/                 # Utility functions (terminal, etc.)
│   ├── ui/                     # UI components (loading, visuals)
│   └── config.c                # General configuration
├── lib/                        # Data structures (queues, stacks, trees)
├── db/                         # Flat file databases
├── bin/                        # Compiled binaries (main.exe)
├── main.c                      # Program entry point
└── README.md                   # Project documentation
```

---

## ⚙️ Compilation

**Using GCC:**

```sh
gcc -I./include -Wall -Wextra -O2 main.c lib/filadupla.c lib/pilha.c lib/fila.c lib/filaprioridade.c lib/arvore_bin.c \
src/app/chamados/extra_functions.c src/app/chamados/chamados.c src/app/chamados/chamadosDb.c \
src/app/menus/menufunctions.c src/app/menus/menu.c src/app/menus/login.c src/app/menus/criar_chamado.c \
src/app/menus/visualizar_cham/fechados.c src/app/menus/visualizar_cham/clientes.c src/app/menus/visualizar_cham/admins.c \
src/app/menus/visualizar_cham/atendentes.c src/app/menus/visualizar_cham/cancelar.c \
src/app/menus/estoque/atualizar_estoque.c src/app/menus/estoque/adicionar_item.c src/app/menus/estoque/deletar_item.c \
src/app/users/userDb.c src/app/estoque/estoque.c src/extras/terminal_utils.c src/extras/utils.c src/ui/carregamento.c src/config.c -o bin/main.exe
```

> The `main.exe` binary will appear in the `bin/` folder.

---

## ▶️ Usage

### Login Credentials

**Clients:**

```
cliente;cliente123
cliente2;cliente123
```

**Technicians:**

```
jonas;gremio
edward;indialove
potter;potter123
```

**Manager / Admin / Mazui (Creator):**

```
gerente;gerente123
admin;admin123
mazui;mazui123
```

> Format: `username;password`

### Running the Program

1. Compile the project using the command above.
2. Run `bin/main.exe`.
3. Navigate menus with arrow keys and confirm with `Enter`.
4. Follow on-screen instructions for creating tickets, managing inventory, and viewing users.

---

## 📝 Notes

* Tickets move to **In Progress** if stock is available; otherwise, they go to **Suspended**.
* Stock updates check pending tickets and prioritize them using a global priority variable.
* Attendants’ ticket queues are managed by workload and ID (lower ID has priority).
* Menus are stack-based, dynamically adapt to terminal size, and are fully interactive.
* Supports Windows (`conio.h`, `windows.h`) and Linux (`termios`, `ioctl`, `signal`).
* Uses ANSI colors and cursor positioning for a polished console UI.

---

## 🌟 Acknowledgements

Developed for the **Data Structures course at UFSM**, demonstrating cross-platform console UI, dynamic ticket handling, and inventory management in C.
