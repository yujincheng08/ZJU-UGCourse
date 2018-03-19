import React from 'react';

export default class Word extends React.Component {
  render() {
    let {word} = this.props.match.params;
    return (
      <div>
        <span>
          {word}
        </span>
      </div>
    );
  }
}