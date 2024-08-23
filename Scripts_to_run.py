import random


common_words = [
    'abandon', 'ability', 'absence', 'account', 'achieve', 'acquire', 'address', 'advance', 'advice', 'agency', 
    'alcohol', 'amount', 'answer', 'appeal', 'apply', 'arrange', 'arrival', 'benefit', 'brother', 'cancer', 
    'capital', 'capture', 'caring', 'circle', 'cloud', 'college', 'combat', 'comment', 'commit', 'common', 
    'create', 'crystal', 'danger', 'debate', 'define', 'degree', 'destroy', 'disease', 'doctor', 'effort', 
    'engage', 'enough', 'family', 'famous', 'friend', 'future', 'garden', 'gather', 'ground', 'happen', 
    'hidden', 'horror', 'impact', 'income', 'injury', 'journey', 'kitchen', 'leader', 'listen', 'manage', 
    'market', 'member', 'mental', 'mystery', 'nature', 'normal', 'office', 'online', 'peace', 'people', 
    'police', 'preserve', 'protect', 'quality', 'reform', 'remain', 'remove', 'secret', 'social', 'spirit', 
    'stable', 'strange', 'strength', 'succeed', 'theory', 'traffic', 'trust', 'unique', 'vacation', 
    'victory', 'village', 'wonder', 'yellow', 'youth', 'accept', 'activity', 'advance', 'airplane', 
    'alright', 'asleep', 'average', 'balance', 'barrier', 'beyond', 'breathe', 'brother', 'capture', 
    'climate', 'concept', 'connect', 'contrast', 'courage', 'danger', 'debate', 'defense', 'deserve', 
    'detect', 'doubt', 'dynamic', 'effort', 'evolve', 'explore', 'freedom', 'generate', 'genuine', 
    'harmony', 'horizon', 'improve', 'inspire', 'justify', 'legacy', 'manage', 'modify', 'neutral', 
    'option', 'patient', 'preview', 'purpose', 'quality', 'remain', 'revenue', 'succeed', 'symbol', 
    'system', 'theory', 'token', 'unfold', 'utility', 'vivid', 'wealth', 'wonder', 'written'
]

def generate_text_file(file_path, num_words=1000):
    """Create a text file with a specified number of meaningful words."""
    if num_words > len(common_words):
        print("Warning: Not enough unique words, allowing duplicates.")
    
    with open(file_path, 'w', encoding='utf-8') as file:
        words = random.choices(common_words, k=num_words) 
        text = ' '.join(words)
        file.write(text)

file_path = 'wordsFile.txt'
generate_text_file(file_path)

print(f"File '{file_path}' with 1,000 meaningful words has been created.")
