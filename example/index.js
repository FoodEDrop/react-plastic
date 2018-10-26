import React from 'react';
import ReactDOM from 'react-dom';
import ReactPlastic from '../';

const App = () => (
  <>
    <h1>Pretend Credit Card</h1>
    <ReactPlastic>
      {({ expiration, handleExpirationChange, handleExpirationKey, getExpirationForDisplay }) => (
        <input
          type="text"
          onChange={handleExpirationChange}
          onKeyDown={handleExpirationKeyDown}
          value={getExpirationForDisplay(expiration)}
        />
      )}
    </ReactPlastic>
  </>
);

ReactDOM.render(<App />, document.getElementById('app'));
