import './App.css';

function App() {
  function handleLogin(e) {
    e.preventDefault();
    e.stopPropagation();
    if (e.target.checkValidity()) {

    } else {
      alert("silahkan isi data");
    }
  }

  return (
    <div className='container'>
      <form onSubmit={(e) => handleLogin(e)} className="card shadow p-5 needs-validation" noValidate={true}>
        <h3 className="text-center">Selamat Datang</h3>
        <div className="form-group">
          <label>Username</label>
          <input type="text" className="form-control" placeholder="Masukan User Name" required />
        </div>
        <div className="form-group">
          <label>Password</label>
          <input type="password" className="form-control" placeholder="Masukan katasandi" required />
        </div>
        <p></p>
        <button type="submit" className="btn btn-primary">Masuk</button>
      </form>
    </div>
  );
}

export default App;
