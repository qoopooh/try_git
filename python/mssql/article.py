#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '0.1'

import sys
import re
from time import strftime, localtime
import pymssql, web
from date import gmt

SQL_SERVER = "erpraid\\sqlexpress"
USER = "cis"
PASSWD = USER
DATABASE = "unikwareData"

UNIK_ARTIKEL = """
SELECT Artikelnummer, ArtMatchcode, ArtBezeichnung1, ArtBezeichnung2
FROM Artikel
WHERE artikelnummer LIKE '{article}%'
"""

UNIK_DETAIL = """
SELECT Artikelnummer AS ItemNumber,
    ArtMatchcode AS Matchcode,
    ISNULL(ArtBezeichnung1, '-') AS Description1,
    ISNULL(ArtBezeichnung2,'-') AS Description2,  
    ISNULL(AlaPhysikalischeMenge1,0) AS InStock,
    ISNULL(AlaVerfuegbar1Menge1,0) AS Available
FROM Artikel   
    LEFT OUTER JOIN ArtikelLager
    ON (ArtikelLager.InterneArtikelnummer = Artikel.InterneArtikelnummer)  
WHERE AlaLagerGruppe IS NULL AND AlaLagerortnummer IS NULL
    AND Artikelnummer = '{article}'
"""

UNIK_USED = """
SELECT a1.Artikelnummer,
    a2.Artikelnummer AS ItemNumber,
    a2.ArtMatchcode AS Matchcode,
    ISNULL(a2.ArtBezeichnung1, '-') AS Description1,
    ISNULL(a2.ArtBezeichnung2,'-') AS Description2
FROM ((Artikel a1
    LEFT OUTER JOIN Stueckliste
    ON (a1.InterneArtikelnummer = StkUnterArtikelnummer))
    INNER JOIN Artikel a2
    ON (a2.InterneArtikelnummer = Stueckliste.InterneArtikelnummer))
WHERE a1.Artikelnummer = '{article}'
ORDER BY a1.Artikelnummer, StkPositionsnummer
"""

UNIK_SUB = """
SELECT a1.Artikelnummer,
    StkPositionsnummer,
    a2.Artikelnummer AS ItemNumber,
    a2.ArtMatchcode AS Matchcode,
    ISNULL(a2.ArtBezeichnung1, '-') AS Description1,
    ISNULL(a2.ArtBezeichnung2,'-') AS Description2,  
    StkMenge1 AS Quantity,
    CASE when(StkMultiplikatorJN = 1) THEN StkMultiplikatorMenge1
    ELSE 1
    END AS Multiplier,
    ISNULL(StkNotiz, '-') AS PositionOnBoard
FROM ((Artikel a1
    LEFT OUTER JOIN Stueckliste
    ON (a1.InterneArtikelnummer = Stueckliste.InterneArtikelnummer))
    INNER JOIN Artikel a2
    ON (a2.InterneArtikelnummer = StkUnterArtikelnummer))
WHERE a1.Artikelnummer = '{article}'
ORDER BY a1.Artikelnummer, StkPositionsnummer
"""

OUTPUT_JS = 'output.js'

def work_time(s, e):
    if e < 0:
        return 0
    if s < 480:
        s = 480
    elif 12*60 < s < 13*60:
        s = 12*60
    w = e-s
    if w < 1:
        return 0
    return w-60

def gen_report(article=None, UNIK_QUERY=UNIK_ARTIKEL):

    count = 0
    rowarray_list = []
    if article == None:
        return rowarray_list

    conn = pymssql.connect(host=SQL_SERVER, user=USER, password=PASSWD,
            database=DATABASE, as_dict=True)
    cur = conn.cursor()
    query = UNIK_QUERY.format(article=article)
    cur.execute(query)
    rows=cur.fetchall()
    conn.close()

    for row in rows:
        t = []
        result = row['Artikelnummer'], row['ArtMatchcode'], \
                 row['ArtBezeichnung1'], row['ArtBezeichnung2']
        t.extend(result)
        rowarray_list.append(t)
        count += 1
        print row

    return rowarray_list

class index:

    def GET(self):
        i = web.input(search=None,group=None)
        return render.article(i.search, i.group, gen_report(i.search))

urls = (
    '/', 'index',
)

template_globals = {
    'app_path': lambda p: web.ctx.homepath + p,
    're': re,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    web.config.debug = True
    app = web.application(urls, globals())
    app.run()

