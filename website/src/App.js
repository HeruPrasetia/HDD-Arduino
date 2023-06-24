import './App.css';
const bootstrap = require('bootstrap')

function App() {
  function handleLoop(val) {
    let html = [];
    for (let i = 0; i < val; i++) html.push(<tr>
      <td>Nama {i}</td>
      <td>File {i}</td>
      <td>Ukuran {i}</td>
      <td>Tanggal {i}</td>
    </tr>)
    return html;
  }

  return (
    <div className='container'>

      <div className="table-responsive">
        <table className='table table-stripped'>
          <thead>
            <tr>
              <th>Nama</th>
              <th>File</th>
              <th>Ukuran</th>
              <th>Tanggal</th>
            </tr>
          </thead>
          <tbody>
            {
              handleLoop(10)
            }
          </tbody>
        </table>
      </div>
    </div>
  );
}

export default App;
