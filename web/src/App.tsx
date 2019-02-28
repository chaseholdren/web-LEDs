import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import LedEditor from './components/LedEditor';

class App extends Component {
  render() {
    return (
      <div className="App">
        <LedEditor/>
      </div>
    );
  }
}

export default App;
