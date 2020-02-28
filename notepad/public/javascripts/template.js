function writeTemplate(author, content, date) {
    return `
        <div class="col-4">
        <div class="card" style="margin: 30px, 10%; width: 18rem;">
            <div class="card-body">
                <h5 class="card-title">${author}</h5>
                <h6 class="card-subtitle mb-2 text-muted">${date}</h6>
                <p class="card-text">${content}</p>
            </div>
        </div>
        </div>
    `;
}