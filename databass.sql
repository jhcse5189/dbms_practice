# create tables
CREATE TABLE master (
  id INT(11) NOT NULL AUTO_INCREMENT,
  model VARCHAR(25) DEFAULT NULL,
  manufacturer VARCHAR(10) DEFAULT NULL,
  nOfStrings TINYINT(1) DEFAULT NULL,
  price INT DEFAULT NULL,
  PRIMARY KEY (id)
);

CREATE TABLE servant (
  id INT(11) NOT NULL AUTO_INCREMENT,
  name VARCHAR(15) DEFAULT NULL,
  PRIMARY KEY (id)
);

CREATE TABLE master_servant_relation (
  sid INT(11) NOT NULL,
  bid INT(11) NOT NULL,
  serveFrom DATETIME NOT NULL,
  PRIMARY KEY (sid, bid, serveFrom),
  FOREIGN KEY (sid) REFERENCES servant (id),
  FOREIGN KEY (bid) REFERENCES master (id)
);

#insert.sql
INSERT INTO master(model, manufacturer, nOfStrings, price) VALUES
("Affinity p-bass", "Squier", 4, 329800),
("G1 SE", "Swing", 4, 327000);

INSERT INTO servant(name) VALUES("Databass");

INSERT INTO master_servant_relation(sid, bid, serveFrom) VALUES
(1, 1, "2018-03-16 00:00:00"),
(1, 2, "2018-07-22 00:00:00");

#select query
SELECT * FROM master;
SELECT * FROM servant;
SELECT * FROM master_servant_relation;

SELECT master.id, model, manufacturer, nOfStrings, price, servant.name AS servant, serveFrom FROM master_servant_relation AS msr LEFT JOIN servant ON msr.sid=servant.id LEFT JOIN master ON msr.bid=master.id;
