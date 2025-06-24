import torch
import sys

# Load the entire model
model = torch.load('cruise_model_hyp.pth', map_location=torch.device('cpu'))
model.eval()  # Set to evaluation mode

def nn(v: float, eps_x: float, eps_v: float):
    input_tensor = torch.tensor([v, eps_x, eps_v], dtype=torch.float)  # Example input

    #print(input_tensor)
    # Run inference
    with torch.no_grad():
        output = model(input_tensor)
        
    return output

if __name__ == "__main__":
    print(nn(float(sys.argv[1]),float(sys.argv[2]), float(sys.argv[3])))
