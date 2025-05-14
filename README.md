# Komma

Short version: **Komma** is a software package that emulates **semantic computer behavior** by storing and processing knowledge in a semantic network. It uses **agent-based approach** to process knowledge graphs.

From the perspective of graph databases, Komma is a **graph database management system** designed to store / retrieve knowledge graphs and run tasks (agents) on them.

Komma stores and processes knowledge graphs represented in the SC-code (language of the universal knowledge representation)..

Both declarative (data, data structures, documentation, problems specification, etc.) and procedural (agents, programs, modules, systems, communication between systems) knowledge is represented using that same language.

Semantic network storage allows integrating problem solutions from different subject domains **by using the same**:

- **technology**
- **programming and data representation language**
- **software stack**.


## This project contains:

- `sc-memory` - semantic storage to store graph constructions;
- `sc-event-manager` - event-based processing (async program workflow handling);
- tools to communicate with `sc-memory`:
  - native C++ API;
  - `sc-server` - a Websocket server that provides a network API to communicate with storage using JSON;
  - `sc-builder` loads information from SCs files into the storage (SCs-code is one of the external representation languages of the SC-code).

## Documentation

- A brief user manual and developer docs are hosted on our [GitHub Pages](https://lindy-son98.github.io/Komma).
  - <details>
      <summary>Build documentation locally</summary>

    ```sh
    # make sure you're using python 3.12
    pip3 install mkdocs mkdocs-material
    mkdocs serve
    # and open http://127.0.0.1:8002/ in your browser
    ```
    </details>
- Full documentation, including:

  - core concepts
  - rationale behind the Komma
  - system design
  - software interfaces
  - more rigorous theoretical calculations


  **Alternatively** you can build Komma documentation yourself. To do that refer to the [scn-latex-plugin](https://github.com/lindy-son98/scn-latex-plugin) documentation.

## Feedback

Contributions, bug reports and feature requests are welcome!
Feel free to check our [issues page](https://github.com/lindy-son98/Komma/issues) and file a new issue (or comment in existing ones).
