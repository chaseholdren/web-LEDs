import React, { ReactHTML } from 'react';
import { firestore } from '../firebaseInit';
import './LedEditor.css';

interface ledData {
    red: number;
    blue: number;
    green: number;
}

interface ledDocument {
    ledData: ledData[];
}



const LedSquare = ({ ledData, onClick }: { ledData: ledData, onClick: () => void }) => {
    const backgroundColor = `rgb(${ledData.red}, ${ledData.green}, ${ledData.blue})`;

    return <div className='led-square' onClick={onClick} style={{ backgroundColor }}></div>
}

type State = {
    brushColor: ledData,
    ledDataList: ledData[],
}

type Props = {

}

class LedEditor extends React.Component<Props, State> {
    state = {
        brushColor: {
            red: 0,
            green: 0,
            blue: 255,
        },
        ledDataList: [],
    }

    unsubscribeFromFirestore?: () => void;

    componentDidMount = () => {
        this.unsubscribeFromFirestore = firestore.collection('ledData').doc("jstJk4unCGziTWsAT3F0").onSnapshot(doc => {
            const ledDocument = doc.data() as ledDocument;
            this.handleUpdateFromFirestore(ledDocument.ledData);
        });
    }

    componentWillUnmount = () => {
        if (this.unsubscribeFromFirestore) {
            this.unsubscribeFromFirestore();
        }
    }

    handleUpdateFromFirestore = (ledDataList: ledData[]) => {
        this.setState({ ledDataList: ledDataList });
    }

    handleLedClick = (index: number) => {
        console.log(index);
        const {ledDataList} = this.state;
        const newLedDataList = Object.assign([], ledDataList) as ledData[];
        newLedDataList[index] = this.state.brushColor;
        firestore.collection("ledData").doc("jstJk4unCGziTWsAT3F0").set({
            ledData: newLedDataList,
        });
    }

    handleCurrentColorChange = (color: string, event: React.ChangeEvent<HTMLInputElement>) => {
        const valueString = event.target.value;
        const value = parseInt(valueString);
        const isValueWithinConstraints = (value >= 0 && value <= 255) || (valueString === '');
        if (!isValueWithinConstraints) return;
        this.setState((prevState) => {
            return {
                ...prevState,
                brushColor: {
                    ...prevState.brushColor,
                    [color]: valueString,
                },
            }
        });
    }

    render = () => {
        const { brushColor } = this.state;
        const currentColor = `rgb(${brushColor.red}, ${brushColor.green}, ${brushColor.blue})`;
        return (
            <div>
                <div className='led-grid'>
                    {this.state.ledDataList.map((ledData, index) =>
                        <LedSquare
                            onClick={() => { this.handleLedClick(index); }}
                            ledData={ledData}
                            key={index}
                        />
                    )}
                </div>
                <div className='color-picker'>
                    <div className='current-color' style={{ backgroundColor: currentColor }}></div>
                    <div className='color-text-inputs'>
                        <div>
                            Red
                            <input type='text' value={brushColor.red} onChange={(event) => { this.handleCurrentColorChange('red', event);}} />
                        </div>
                        <div>
                            Green
                            <input type='text' value={brushColor.green} onChange={(event) => { this.handleCurrentColorChange('green', event); }} />
                        </div>
                        <div>
                            Blue
                            <input type='text' value={brushColor.blue} onChange={(event) => { this.handleCurrentColorChange('blue', event); }} />
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}

export default LedEditor;
