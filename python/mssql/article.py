#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0'

import sys
import pymssql, web

SQL_SERVER = "erpraid\\sqlexpress"
USER = "cis"
PASSWD = USER
DATABASE = "unikwareData"

UNIK_ARTIKEL = """
SELECT Artikelnummer, ArtMatchcode, ArtBezeichnung1, ArtBezeichnung2
FROM Artikel
WHERE artikelnummer LIKE %s
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
    AND Artikelnummer = %s
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
WHERE a1.Artikelnummer = %s
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
    CASE WHEN (StkMultiplikatorJN = 1)
        THEN StkMultiplikatorMenge1               
        ELSE 1                                                                      
        END AS Multiplier,                                                          
    ISNULL(StkNotiz, '-') AS PositionOnBoard,
    ISNULL(AlaPhysikalischeMenge1,0) AS InStock,
    ISNULL(AlaVerfuegbar1Menge1,0) AS Available
    FROM ((Artikel a1                                                               
        LEFT OUTER JOIN Stueckliste                                                 
        ON (a1.InterneArtikelnummer = Stueckliste.InterneArtikelnummer))            
        INNER JOIN Artikel a2                                                       
        ON (a2.InterneArtikelnummer = StkUnterArtikelnummer))                       
        LEFT OUTER JOIN ArtikelLager ON (ArtikelLager.InterneArtikelnummer = a2.InterneArtikelnummer) 
    WHERE AlaLagerGruppe IS NULL AND AlaLagerortnummer IS NULL
        AND a1.Artikelnummer = %s
    ORDER BY a1.Artikelnummer, StkPositionsnummer
"""

def gen_report(article=None, UNIK_QUERY=UNIK_ARTIKEL):

    count = 0
    rowarray_list = []
    if article == None:
        return rowarray_list

    conn = pymssql.connect(host=SQL_SERVER, user=USER, password=PASSWD,
            database=DATABASE, as_dict=True)
    cur = conn.cursor()
    if UNIK_QUERY == UNIK_ARTIKEL:
        cur.execute(UNIK_QUERY, (article + '%'))
    else:
        cur.execute(UNIK_QUERY, (article))
    rows=cur.fetchall()
    conn.close()

    for row in rows:
        count += 1
        t = []
        if UNIK_QUERY == UNIK_DETAIL:
            result = row['Matchcode'], row['Description1'], \
                     row['Description2'], row['InStock'], row['Available']
        elif UNIK_QUERY == UNIK_USED:
            result = count, row['ItemNumber'], row['Matchcode'], \
                     row['Description1'], row['Description2']
        elif UNIK_QUERY == UNIK_SUB:
            result = count, row['ItemNumber'], row['Matchcode'], \
                     row['Description1'], row['Description2'], \
                     row['Quantity'], row['Multiplier'], \
                     row['InStock'], row['Available'], \
                     row['PositionOnBoard']
        else:
            result = count, row['Artikelnummer'], row['ArtMatchcode'], \
                     row['ArtBezeichnung1'], row['ArtBezeichnung2']
        t.extend(result)
        rowarray_list.append(t)
        print row

    return rowarray_list

class index:

    def GET(self):
        i = web.input(search='880-00',group=None)
        return render.article(i.search, i.group, gen_report(i.search))

class Info:

    def GET(self):
        i = web.input(a=None)
        return render.article_info(i.a, \
                gen_report(i.a, UNIK_DETAIL),
                gen_report(i.a, UNIK_USED),
                gen_report(i.a, UNIK_SUB))

urls = (
    '/', 'index',
    '/info', 'Info',
)

template_globals = {
    'version': __version__,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    web.config.debug = True
    app = web.application(urls, globals())
    app.run()

