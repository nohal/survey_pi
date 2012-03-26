CREATE TABLE survey (
survey_id INTEGER PRIMARY KEY AUTOINCREMENT,
survey_name TEXT,
created INTEGER,
submitted INTEGER);


CREATE TABLE sounding (
sounding_id INTEGER PRIMARY KEY AUTOINCREMENT,
depth REAL NOT NULL,
measured INTEGER NOT NULL,
survey_id INTEGER NOT NULL,
FOREIGN KEY(survey_id) REFERENCES survey(survey_id));

SELECT AddGeometryColumn('sounding', 'geom', 32632, 'POINT', 2);

INSERT INTO "survey" ("survey_name", "created", "submitted") VALUES ("test survey", current_timestamp, NULL);

INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.527777778 , current_timestamp, 1, GeomFromText('POINT(37.87553333 -122.31705)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.5 , current_timestamp, 1, GeomFromText('POINT(37.87543333 -122.3167667)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.472222222 , current_timestamp, 1, GeomFromText('POINT(37.87476667 -122.3163167)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.444444444 , current_timestamp, 1, GeomFromText('POINT(37.87388333 -122.31565)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.5 , current_timestamp, 1, GeomFromText('POINT(37.87305 -122.3149667)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.472222222 , current_timestamp, 1, GeomFromText('POINT(37.87221667 -122.3143833)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.361111111 , current_timestamp, 1, GeomFromText('POINT(37.87135 -122.3138)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.277777778 , current_timestamp, 1, GeomFromText('POINT(37.87076667 -122.3136667)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.083333333 , current_timestamp, 1, GeomFromText('POINT(37.87015 -122.3132333)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.055555556 , current_timestamp, 1, GeomFromText('POINT(37.87006667 -122.3127)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.083333333 , current_timestamp, 1, GeomFromText('POINT(37.87013333 -122.3122833)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.25 , current_timestamp, 1, GeomFromText('POINT(37.87038333 -122.3117333)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.305555556 , current_timestamp, 1, GeomFromText('POINT(37.87066667 -122.3113333)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.25 , current_timestamp, 1, GeomFromText('POINT(37.8708 -122.31075)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.138888889 , current_timestamp, 1, GeomFromText('POINT(37.87088333 -122.3103333)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.083333333 , current_timestamp, 1, GeomFromText('POINT(37.8711 -122.31005)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.111111111 , current_timestamp, 1, GeomFromText('POINT(37.8713 -122.3096667)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.305555556 , current_timestamp, 1, GeomFromText('POINT(37.87156667 -122.3103)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.444444444 , current_timestamp, 1, GeomFromText('POINT(37.87181667 -122.3115)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.527777778 , current_timestamp, 1, GeomFromText('POINT(37.8721 -122.3125833)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.611111111 , current_timestamp, 1, GeomFromText('POINT(37.87265 -122.31355)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.666666667 , current_timestamp, 1, GeomFromText('POINT(37.87298333 -122.31425)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.75 , current_timestamp, 1, GeomFromText('POINT(37.87395 -122.3144833)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.833333333 , current_timestamp, 1, GeomFromText('POINT(37.87471667 -122.3135667)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.777777778 , current_timestamp, 1, GeomFromText('POINT(37.87536667 -122.3126167)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.694444444 , current_timestamp, 1, GeomFromText('POINT(37.875 -122.3114)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.583333333 , current_timestamp, 1, GeomFromText('POINT(37.87428333 -122.3105667)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.722222222 , current_timestamp, 1, GeomFromText('POINT(37.87581667 -122.3142333)', 32632));
INSERT INTO "sounding" ("depth", "measured", "survey_id", "geom") VALUES (2.916666667 , current_timestamp, 1, GeomFromText('POINT(37.87645 -122.3158167)', 32632));

