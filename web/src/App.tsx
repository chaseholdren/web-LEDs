import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import LedEditor from './components/LedEditor';

class App extends Component {
  render() {
    return (
      <div className="App">
        <LedEditor/>
        <div className="Pics">
          <img src="/img/1.jpg" alt="LED Grid" />
          <img src="/img/2.jpg" alt="LED Grid" />
        </div>
      </div>
    );
  }
}

export default App;
