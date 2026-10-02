#!/usr/bin/env python3
"""
osm_para_grafo.py — Converte um extrato .osm (XML do Overpass) para o
formato edge list do projeto (ver documentacao/formato_entrada.md).

Uso: python3 scripts/osm_para_grafo.py entrada.osm saida.txt
"""
import sys
import xml.etree.ElementTree as ET

VIAS_CARRO = {
    "motorway", "trunk", "primary", "secondary", "tertiary",
    "unclassified", "residential", "service", "road", "living_street",
    "motorway_link", "trunk_link", "primary_link", "secondary_link",
    "tertiary_link",
}

def eh_mao_unica(tags, junction):
    ow = tags.get("oneway", "").lower()
    if ow in ("yes", "true", "1") or junction == "roundabout":
        return "frente"
    if ow == "-1":
        return "tras"
    return "ambos"

def main():
    entrada, saida = sys.argv[1], sys.argv[2]
    arvore = ET.parse(entrada)
    raiz = arvore.getroot()

    arestas = []
    vertices_usados = set()

    for way in raiz.iter("way"):
        tags = {t.get("k"): t.get("v") for t in way.iter("tag")}
        if tags.get("highway") not in VIAS_CARRO:
            continue
        nos = [nd.get("ref") for nd in way.iter("nd")]
        direcao = eh_mao_unica(tags, tags.get("junction"))
        for a, b in zip(nos, nos[1:]):
            if direcao in ("frente", "ambos"):
                arestas.append((a, b))
                vertices_usados.update((a, b))
            if direcao in ("tras", "ambos"):
                arestas.append((b, a))
                vertices_usados.update((a, b))

    # mapeia ids OSM -> 0..n-1
    mapa = {osm_id: i for i, osm_id in enumerate(sorted(vertices_usados))}

    with open(saida, "w") as f:
        f.write(f"{len(mapa)} {len(arestas)}\n")
        for a, b in arestas:
            f.write(f"{mapa[a]} {mapa[b]}\n")

    print(f"Vertices: {len(mapa)} | Arestas: {len(arestas)} -> {saida}")

if __name__ == "__main__":
    main()
