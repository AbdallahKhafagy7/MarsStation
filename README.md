# 🚀 Mars Station – DSA Project

Simulates a **Mars mission control system** that manages rovers performing various missions across the Martian surface.  
Built with **C++**, this project demonstrates practical use of **data structures** such as stacks, queues, and priority queues in a realistic simulation scenario.

---

## 🔹 Project Overview
The Mars Station project manages multiple rovers (Digging, Polar, Normal) and assigns them to missions based on type and priority. The system tracks mission progress, handles abort requests, and generates detailed statistics to help optimize mission planning.

**Mission Flow:**  
`Ready → Launched → Executing → Back → Done`

---

## ⚡ Key Features

- **Mission Management**
  - Digging (DM), Polar (PM), Normal (NM) missions
  - Tracks mission states and progress
- **Rover Management**
  - DR, PR, NR rovers with speed, checkup, and availability
- **Intelligent Assignment**
  - Priority-based mission scheduling
  - Handles abort requests automatically for delayed polar missions
- **Simulation Modes**
  - **Interactive Mode:** Step-by-step simulation with console output
  - **Silent Mode:** Minimal output, generates a complete results file
- **Statistics Generation**
  - Mission & rover counts
  - Average waiting time, mission duration, total mission time
  - Auto-aborted polar mission percentage

---

## 📂 Solution Tree / Project Structure

```
DSA Project (Visual Studio Solution)
│
├─ Data Structures
│  ├─ OUT_missions.h
│  ├─ PriQueue/
│  │  ├─ PriNode.h
│  │  └─ PriQueue.h
│  ├─ Queue/
│  │  ├─ LinkedQueue.h
│  │  └─ Node.h
│  ├─ RDY_NM.h
│  ├─ Stack/
│  │  └─ ArrayStack.h
│  └─ Tests/
│     └─ Test.h
│
├─ MarsStation/
│  ├─ MarsStation.cpp
│  └─ MarsStation.h
│
├─ mission/
│  ├─ mission.cpp
│  └─ mission.h
│
├─ request/
│  ├─ abortRequest.cpp
│  ├─ abortRequest.h
│  ├─ newRequest.cpp
│  ├─ newRequest.h
│  └─ request.h
│
├─ rover/
│  ├─ rover.cpp
│  └─ rover.h
│
├─ UI/
│  ├─ UI.cpp
│  └─ UI.h
│
├─ Source Files
│  └─ main.cpp
│
└─ input.txt
```

> 💡 `Tests` folder is included under **Data Structures** to validate stacks, queues, and priority queues.

---

## 📝 Input / Output Format

**Input (`input.txt`):**
```
D P N          // Number of DRs, PRs, NRs
SD SP SN       // Speeds of DR, PR, NRs
M CD CP CN     // Missions before checkup, checkup durations
K              // Number of requests
R TYP Rday ID TLOC DUR   // New mission
X XD ID                  // Abort mission
```

**Output Example:**
```
Fday ID Rday Wdays MDUR Tdays
```
Followed by statistics for all missions and rovers.

---

## 🔄 Simulation Logic

- **Time-step:** 1 day per iteration
- **Daily Operations:**
  1. Receive new missions and abort requests
  2. Assign ready missions to rovers based on rules
  3. Update mission states as rovers move and execute missions
  4. Track rover checkups and availability

- **Mission Assignment Rules:**
  1. PM → PR → NR → DR
  2. DM → DR only
  3. NM → NR → PR
  4. Wait if no suitable rover available

---

## ⚙️ Algorithms & Data Structures

| Structure | Purpose |
|-----------|---------|
| **Stack** | Temporary mission storage |
| **Queue** | Ready mission management (FIFO) |
| **Priority Queue** | Priority-based mission scheduling |
| **Linked List** | Dynamic mission and rover storage |
| **Unit Tests** | Validate correctness of data structures |

---

## ▶ How to Run

### Visual Studio
1. Open `DSA Project.sln`
2. Set `DSA Project` as the startup project
3. Build and run (`F5`)
4. Input can be provided via `input.txt` or interactively

### GCC / Other Compilers
```bash
g++ *.cpp -o MarsStation
./MarsStation          # Run normally
./MarsStation < input.txt  # Redirect input from file
```
