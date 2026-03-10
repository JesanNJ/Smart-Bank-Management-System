import sys
import json
import urllib.request
import urllib.error
import os

def call_ai(api_key, system_msg, user_msg):
    # Standardizing on Gemini 3 Flash Preview
    url = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-3-flash-preview:generateContent?key={api_key}"
    headers = {"Content-Type": "application/json"}
    
    data = {
        "contents": [{"parts": [{"text": user_msg}]}],
        "system_instruction": {"parts": [{"text": system_msg}]},
        "generationConfig": {
            "temperature": 0.2,
            "topP": 0.8,
            "topK": 40,
            "maxOutputTokens": 1024,
        }
    }
    
    req = urllib.request.Request(url, data=json.dumps(data).encode('utf-8'), headers=headers, method='POST')
    try:
        with urllib.request.urlopen(req) as resp:
            body = resp.read().decode('utf-8')
            res_json = json.loads(body)
            if "candidates" in res_json and len(res_json["candidates"]) > 0:
                content = res_json["candidates"][0]["content"]["parts"][0]["text"]
                sys.stdout.write(content)
                return True
            else:
                sys.stderr.write(f"Gemini API returned no content. Response: {body}\n")
                return False
    except urllib.error.HTTPError as e:
        error_content = e.read().decode('utf-8')
        sys.stderr.write(f"Gemini HTTP Error {e.code}: {error_content}\n")
        return False
    except Exception as e:
        sys.stderr.write(f"Python Bridge Error: {str(e)}\n")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(1)
    
    input_file = sys.argv[1]
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            config = json.load(f)
        
        system_msg = config.get("system_message", "")
        user_msg = config.get("user_message", "")
        # Prioritize key from environment (more secure), then JSON
        api_key = os.getenv("GEMINI_API_KEY") or config.get("api_key")
        
        if not api_key:
            sys.stderr.write("Error: GEMINI_API_KEY not found in environment or config.\n")
            sys.exit(1)

        if not call_ai(api_key, system_msg, user_msg):
            sys.exit(1)
            
    except Exception as e:
        sys.stderr.write(f"Error reading bridge input: {str(e)}\n")
        sys.exit(1)
