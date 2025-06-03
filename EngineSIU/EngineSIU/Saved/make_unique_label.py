from dataclasses import dataclass
from json import load, dumps
from argparse import ArgumentParser
from pathlib import Path


@dataclass
class Actor:
    ActorClass: str
    ActorID: str
    ActorLabel: str
    ActorTickInEditor: str
    Components: list[dict]
    Properties: dict
    RootComponentID: str

@dataclass
class Scene:
    Version: int
    NextUUID: int
    GameModeName: str
    Actors: list[Actor]


def load_scene_from_json(file_path: str) -> Scene:
    with open(file_path, 'r') as file:
        data = load(file)
    
    actors = [Actor(**actor) for actor in data['Actors']]
    return Scene(
        Version=data['Version'],
        NextUUID=data['NextUUID'],
        GameModeName=data['GameModeName'],
        Actors=actors
    )

def get_id(actor_id: str) -> int:
    return int(actor_id.split('_')[-1])

def main(file_path: str):
    val = load_scene_from_json(file_path)

    unique_actors = set()

    for actor in val.Actors:
        if actor.ActorLabel not in unique_actors:
            unique_actors.add(actor.ActorLabel)
            print(f"Actor ID: {actor.ActorLabel}, Class: {actor.ActorClass}, Label: {actor.ActorLabel}")
        else:
            print(f"Duplicate Actor ID found: {actor.ActorLabel}, Class: {actor.ActorClass}, Label: {actor.ActorLabel}")
            new_id = actor.ActorLabel[:actor.ActorLabel.rfind("_")]  + f"_{get_id(actor.ActorLabel)+1}"
            while new_id in unique_actors:
                new_id = actor.ActorLabel[:actor.ActorLabel.rfind("_")] + f"_{get_id(new_id)+1}"
            unique_actors.add(new_id)
            print(f"Actor ID: {new_id}, Class: {actor.ActorClass}, Label: {actor.ActorLabel} (renamed)")
            actor.ActorLabel = new_id
            
    print(f"Total unique actors: {len(unique_actors)}")
    with open(f"./Updated_{Path(file_path).name}", 'w') as file:
        file.write(dumps(val, indent=4, default=lambda o: o.__dict__))

    print("Done processing actors.")


if __name__ == "__main__":
    parser = ArgumentParser(description="Process game scene actors to ensure unique labels.")
    parser.add_argument('file', nargs='?', type=str, help='Path to the scene file.')
    args = parser.parse_args()
    # main("GameScene1.scene")  # Default file path for testing
    if args.file:
        main(args.file)
    else:
        print("Please provide a file path to the scene file.")
        parser.print_help()
        exit(1)
