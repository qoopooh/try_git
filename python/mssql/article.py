#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.2'

import sys, re
import pymssql, web

SQL_SERVER = "erpraid\\sqlexpress"
USER = "cis"
PASSWD = USER
DATABASE = "unikwareData"

UNIK_ARTIKEL = """
SELECT Artikelnummer, ArtMatchcode, ArtBezeichnung1, ArtBezeichnung2
FROM Artikel
WHERE artikelnummer LIKE %s
ORDER BY Artikelnummer
"""

UNIK_ALLSEARCH = """
SELECT Artikelnummer, ArtMatchcode, ArtBezeichnung1, ArtBezeichnung2
FROM Artikel
WHERE Artikelnummer LIKE %s
    OR ArtMatchcode LIKE %s
    OR ArtBezeichnung1 LIKE %s
    OR ArtBezeichnung2 LIKE %s
ORDER BY Artikelnummer
"""

UNIK_SEARCH = """
SELECT Artikelnummer, ArtMatchcode, ArtBezeichnung1, ArtBezeichnung2
FROM Artikel
WHERE {group} LIKE %s
ORDER BY Artikelnummer
"""

ARTIKEL = "artikelnummer"
MATCHCODE = "ArtMatchcode"
DESCRIPTION1 = "ArtBezeichnung1"
DESCRIPTION2 = "ArtBezeichnung2"

UNIK_DETAIL = """
SELECT Artikelnummer AS ItemNumber,
    ArtMatchcode AS Matchcode,
    ISNULL(ArtBezeichnung1, '-') AS Description1,
    ISNULL(ArtBezeichnung2,'-') AS Description2,  
    ISNULL(AlaPhysikalischeMenge1,0) AS InStock,
    ISNULL(AlaVerfuegbar1Menge1,0) AS Available,
    ISNULL(KZArtMengeneinheit1, '-') AS Unit,
    DokDatei, DokOriginalDatei
FROM Artikel   
    LEFT OUTER JOIN ArtikelLager
    ON (ArtikelLager.InterneArtikelnummer = Artikel.InterneArtikelnummer)
    LEFT OUTER JOIN DokumentArtikel
    ON (DokumentArtikel.InterneArtikelnummer = Artikel.InterneArtikelnummer)
    LEFT OUTER JOIN Dokumente
    ON (Dokumente.DokumentID = DokumentArtikel.DokumentID)
WHERE AlaLagerGruppe IS NULL AND AlaLagerortnummer IS NULL
    AND Artikelnummer = %s
"""

UNIK_DETAIL_IMAGE = """
SELECT DokumentArtikel.InterneArtikelnummer, DokumentArtikel.DokumentID, Dokumente.DokDatei
FROM Artikel, DokumentArtikel, Dokumente
WHERE Artikelnummer= %s
    AND Artikel.InterneArtikelnummer=DokumentArtikel.InterneArtikelnummer
    AND DokumentArtikel.DokumentID=Dokumente.DokumentID
    AND DokDatei LIKE '%.jpg'
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
    ISNULL(a2.KZArtMengeneinheit1, '-') AS Unit,
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

def gen_report(article, UNIK_QUERY, group='all'):

    count = 0
    rowarray_list = []
    if article == None:
        return rowarray_list

    conn = pymssql.connect(host=SQL_SERVER, user=USER, password=PASSWD,
            database=DATABASE, as_dict=True)
    cur = conn.cursor()
    if UNIK_QUERY == UNIK_ARTIKEL:
        cur.execute(UNIK_QUERY, (article + '%'))
    elif UNIK_QUERY == UNIK_ALLSEARCH:
        s = '%' + article + '%'
        cur.execute(UNIK_QUERY, (s,s,s,s))
    elif UNIK_QUERY == UNIK_SEARCH:
        s = '%' + article + '%'
        cur.execute(UNIK_QUERY.format(group=group), (s))
    else:
        cur.execute(UNIK_QUERY, (article))
    rows=cur.fetchall()
    conn.close()

    for row in rows:
        count += 1
        t = []
        if UNIK_QUERY == UNIK_DETAIL:
            result = row['Matchcode'], row['Description1'], \
                     row['Description2'], row['InStock'], row['Available'], \
                     row['Unit'], \
                     (row['DokDatei'], row['DokOriginalDatei'])
        elif UNIK_QUERY == UNIK_USED:
            result = count, row['ItemNumber'], row['Matchcode'], \
                     row['Description1'], row['Description2']
        elif UNIK_QUERY == UNIK_SUB:
            result = count, row['ItemNumber'], row['Matchcode'], \
                     row['Description1'], row['Description2'], \
                     row['Quantity'], row['Unit'], row['Multiplier'], \
                     row['InStock'], row['Available'], \
                     row['PositionOnBoard']
        else:
            result = count, row['Artikelnummer'], row['ArtMatchcode'], \
                     row['ArtBezeichnung1'], row['ArtBezeichnung2']
        t.extend(result)
        rowarray_list.append(t)
        print row

    return rowarray_list

def gen_bom(article, rev):
    bom = {'rev': '0', 'name': '', 'date': '2014-06-30', 'header': [], \
        'info': []}

    return bom

class index:

    def GET(self):
        i = web.input(search='900-', group='article')
        length = len(i.search)
        a = '-'
        if i.group == 'article':
            s = i.search.strip()
            length = len(s)
            if length > 3 and s[3] != '-' \
                    and bool(re.compile('^\d{3}').search(s)):
                a = s[:3] + '-' + s[3:]
            elif length > 1:
                a = s
            report = gen_report(a, UNIK_ARTIKEL)
            return render.article(a, i.group, report)

        a = self.filter_word(a, length, i.search)
        if i.group == 'matchcode':
            report = gen_report(a, UNIK_SEARCH, MATCHCODE)
        elif i.group == 'desc1':
            report = gen_report(a, UNIK_SEARCH, DESCRIPTION1)
        elif i.group == 'desc2':
            report = gen_report(a, UNIK_SEARCH, DESCRIPTION2)
        else:
            report = gen_report(a, UNIK_ALLSEARCH)
        return render.article(a, i.group, report)

    def filter_word(self, a, ln, s):
        if ln > 1:
            a = s
        else:
            a = 'at_least_two_chars'
        return a

class Info:

    def GET(self):
        i = web.input(a=None)
        return render.article_info(i.a, \
                gen_report(i.a, UNIK_DETAIL),
                gen_report(i.a, UNIK_USED),
                gen_report(i.a, UNIK_SUB))

class Bom:

    def GET(self):
        i = web.input(a=None, rev=None)
        return render.article_bom(i.a, \
                gen_bom(i.a, i.rev))

urls = (
    '/', 'index',
    '/info', 'Info',
    '/bom', 'Bom',
)

template_globals = {
    'version': __version__,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    web.config.debug = True
    app = web.application(urls, globals())
    app.run()

