/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qneconnection.h"

#include "qneport.h"
#include "qneblock.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

#include <assert.h>

#include "DspComponent.h"

QNEConnection::QNEConnection(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
	setPen(QPen(Qt::black, 2));
	setBrush(Qt::NoBrush);
	setZValue(-1);
	m_port1 = 0;
	m_port2 = 0;
}

void QNEConnection::updateCircuitConnection()
{  
  if (m_port1 && m_port2) {
    DspComponent *comp_source, *comp_sink;
    DspCircuit *circ_sink, *circ_source;
    DspCircuit *circuit = NULL;
    QNEPort *src, *sink;
    bool success;
    
    if (m_port1->isOutput()) {
      src = m_port1;
      sink = m_port2;
    }
    else {
      sink = m_port2;
      src = m_port1;
    }
    
    //only one may be 
    assert((src->isOutput() || sink->isOutput()) && (src->isOutput() != sink->isOutput()));
    
    comp_source = m_port1->block()->component;
    comp_sink = m_port2->block()->component;
    circ_source = m_port1->block()->circuit;
    circ_sink = m_port2->block()->circuit;
    if (comp_sink)
      circuit = comp_sink->GetParentCircuit();
    int source_idx = m_port1->block()->getPortIdx(m_port1);
    int sink_idx = m_port2->block()->getPortIdx(m_port2);
    
    if (comp_source && comp_sink)
      circuit->ConnectOutToIn(comp_source,
                              source_idx,
                              comp_sink,
                              sink_idx
                            );
    else if (circ_source && comp_sink) {
      //FIXME detect whats input and whats output!
      printf("connect input\n");
      success = circ_source->ConnectInToIn(source_idx, comp_sink, sink_idx);
      assert(success);
    }
    else if (comp_source && circ_sink) {
      //FIXME detect whats input and whats output!
      printf("connect output\n");
      success = circ_sink->ConnectOutToOut(comp_source, source_idx, sink_idx);
      assert(success);
    }
    else 
      //not yet supported by DSPatch?
      abort();
  }
}


void QNEConnection::removeCircuitConnection()
{  
  if (m_port1 && m_port2) {
    DspComponent *comp_source, *comp_sink;
    DspCircuit *circ_sink, *circ_source;
    DspCircuit *circuit = NULL;
    QNEPort *src, *sink;
    bool success;
    
    if (m_port1->isOutput()) {
      src = m_port1;
      sink = m_port2;
    }
    else {
      sink = m_port2;
      src = m_port1;
    }
    
    //only one may be 
    assert((src->isOutput() || sink->isOutput()) && (src->isOutput() != sink->isOutput()));
    
    comp_source = m_port1->block()->component;
    comp_sink = m_port2->block()->component;
    circ_source = m_port1->block()->circuit;
    circ_sink = m_port2->block()->circuit;
    if (comp_sink)
      circuit = comp_sink->GetParentCircuit();
    int source_idx = m_port1->block()->getPortIdx(m_port1);
    int sink_idx = m_port2->block()->getPortIdx(m_port2);
    
    if (comp_source && comp_sink)
      circuit->DisconnectOutToIn(comp_source,
                              source_idx,
                              comp_sink,
                              sink_idx
                            );
    else if (circ_source && comp_sink) {
      circ_source->DisconnectInToIn(source_idx, comp_sink, sink_idx);
    }
    else if (comp_source && circ_sink) {
      circ_sink->DisconnectOutToOut(comp_source, source_idx, sink_idx);
    }
    else 
      //not yet supported by DSPatch?
      abort();
  }
}

QNEConnection::~QNEConnection()
{
        //FIXME add clear function?!
        //removeCircuitConnection();
        
	if (m_port1)
		m_port1->connections().remove(m_port1->connections().indexOf(this));
	if (m_port2)
		m_port2->connections().remove(m_port2->connections().indexOf(this));
}

void QNEConnection::setPos1(const QPointF &p)
{
	pos1 = p;
}

void QNEConnection::setPos2(const QPointF &p)
{
	pos2 = p;
}

void QNEConnection::setPort1(QNEPort *p, bool gui_only)
{
	m_port1 = p;

	m_port1->connections().append(this);
        
        if (!gui_only)
          updateCircuitConnection();
}

void QNEConnection::setPort2(QNEPort *p, bool gui_only)
{
	m_port2 = p;

	m_port2->connections().append(this);
        
        if (!gui_only)
          updateCircuitConnection();
}

void QNEConnection::updatePosFromPorts()
{
	pos1 = m_port1->scenePos();
	pos2 = m_port2->scenePos();
}

void QNEConnection::updatePath()
{
	QPainterPath p;

	//QPointF pos1(m_port1->scenePos());
	//QPointF pos2(m_port2->scenePos());

	p.moveTo(pos1);

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

	QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
	QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

	p.cubicTo(ctr1, ctr2, pos2);

	setPath(p);
}

QNEPort* QNEConnection::port1() const
{
	return m_port1;
}

QNEPort* QNEConnection::port2() const
{
	return m_port2;
}

void QNEConnection::save(QDataStream &ds)
{
	ds << (quint64) m_port1;
	ds << (quint64) m_port2;
}

void QNEConnection::load(QDataStream &ds, const QMap<quint64, QNEPort*> &portMap)
{
	quint64 ptr1;
	quint64 ptr2;
	ds >> ptr1;
	ds >> ptr2;

	setPort1(portMap[ptr1]);
	setPort2(portMap[ptr2]);
	updatePosFromPorts();
	updatePath();
}
