import React, {useState} from 'react';
import { Modal, Button } from 'react-bootstrap';
function Popup(props) {
    return (
        <Modal show={props.showPopUp} onHide={props.handlePopUpClose} animation={false}>
            <Modal.Header closeButton>
            <Modal.Title>{props.Type}</Modal.Title>
            </Modal.Header>
            <Modal.Body>{props.Message}</Modal.Body>
            <Modal.Footer>
            <Button variant="secondary" onClick={props.handlePopUpClose}>
                Inchide
            </Button>
            <Button variant="primary" onClick={props.handlePopUpClose}>
                Ok
            </Button>
            </Modal.Footer>
        </Modal>
    );
}

export default Popup;