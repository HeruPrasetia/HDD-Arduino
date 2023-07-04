import './App.css';
import React, { useState } from 'react';
import { getCookie, setCookie, api } from "./component/Modul";

function App() {
  let Token = getCookie("TokenHDD");

  const [contextMenuVisible, setContextMenuVisible] = useState(false);
  const [contextMenuPosition, setContextMenuPosition] = useState({ x: 0, y: 0 });

  const handleContextMenu = (e) => {
    e.preventDefault();
    const posX = e.clientX;
    const posY = e.clientY;
    setContextMenuPosition({ x: posX, y: posY });
    setContextMenuVisible(true);
  };

  const hideContextMenu = () => {
    setContextMenuVisible(false);
  };


  if (Token) {
    return (
      <div className="finder">
        <div className="finder-header">
          <div className="finder-title">Finder</div>
          <div className="finder-controls">
            <button className="finder-control btn btn-secondary">Back</button>
            <button className="finder-control btn btn-secondary">Forward</button>
            <button className="finder-control btn btn-secondary">Up</button>
          </div>
        </div>
        <table className="finder-table">
          <thead>
            <tr>
              <th>Nama</th>
              <th>Ukuran</th>
              <th>Tanggal</th>
              <th>Tipe</th>
            </tr>
          </thead>
          <tbody>
            <tr className="finder-item" onContextMenu={handleContextMenu}>
              <td>
                <div className="finder-text">Folder 1</div>
              </td>
              <td>123 KB</td>
              <td>2023-06-25</td>
              <td>Folder</td>
            </tr>
            <tr className="finder-item" onContextMenu={handleContextMenu}>
              <td>
                <div className="finder-text">File 1.txt</div>
              </td>
              <td>456 KB</td>
              <td>2023-06-26</td>
              <td>Text File</td>
            </tr>
            <tr className="finder-item" onContextMenu={handleContextMenu}>
              <td>
                <div className="finder-text">File 2.txt</div>
              </td>
              <td>789 KB</td>
              <td>2023-06-27</td>
              <td>Text File</td>
            </tr>
          </tbody>
        </table>
        {contextMenuVisible && (
          <div
            className="context-menu"
            style={{
              left: contextMenuPosition.x,
              top: contextMenuPosition.y,
            }}
            onClick={hideContextMenu}
          >
            <ul>
              <li>Open</li>
              <li>Copy</li>
              <li>Delete</li>
            </ul>
          </div>
        )}
      </div>
    );
  } else {
    return (
      <div className='container'>
        <form onSubmit={(e) => handleLogin(e)} className="card shadow p-5 needs-validation" noValidate={true}>
          <h3 className="text-center">Selamat Datang</h3>
          <div className="form-group">
            <label>Username</label>
            <input type="text" id="edtUserName" className="form-control" placeholder="Masukan User Name" required />
            <div className="invalid-feedback">Silahkan masukan Username</div>
          </div>
          <div className="form-group">
            <label>Password</label>
            <input type="password" id="edtPwd" className="form-control" placeholder="Masukan katasandi" required />
            <div className="invalid-feedback">Silahkan masukan Password</div>
          </div>
          <p></p>
          <div className="alert alert-danger" role="alert" id="divAlert" style={{ display: "none" }}></div>
          <button type="submit" className="btn btn-primary">Masuk</button>
        </form>
      </div>
    );
  }
}

function handleLogin(e) {
  e.preventDefault();
  e.stopPropagation();
  if (e.target.checkValidity()) {
    let UserName = document.getElementById('edtUserName').value;
    let Pwd = document.getElementById('edtPwd').value;
    let alert = document.getElementById('divAlert');
    if (Pwd = 'superadmin') {
      if (UserName == "user") {
        alert.style.display = "none";
        setCookie("TokenHDD", "iniadalahtoken");
        window.location.reload();
      } else {
        alert.style.display = "block";
        alert.innerText = "Username Salah";
      }
    } else {
      alert.style.display = "block";
      alert.innerText = "Password Salah";
    }
  } else {
    let forms = document.getElementsByClassName('needs-validation');
    let validation = Array.prototype.filter.call(forms, function (form) {
      form.classList.add('was-validated');
    });
  }
}

export default App;
